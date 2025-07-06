import React, { useEffect, useState } from 'react';
import { Container, Typography, Alert } from '@mui/material';
import {
  startJob, pauseJob, resumeJob, stopJob, getSockets, getJobStatus,
  getReadyToPick, getReadyToPlace, placeDevice, pickDevice
} from './services/api';
import JobControlPanel from './components/JobControlPanel';
import SocketGrid from './components/SocketGrid';
import StatsDisplay from './components/StatsDisplay';

const fetchAndSetAllSockets = async (setSocketsFn) => {
  try {
    const [site1Res, site2Res] = await Promise.all([
      getSockets(0),
      getSockets(1)
    ]);

    const combined = [];

    for (const res of [site1Res, site2Res]) {
      const siteData = res.data.data;
      if (siteData && siteData.length > 0) {
        const site = siteData[0];
        if (site && Array.isArray(site.sockets)) {
          const flatSockets = site.sockets.map((socket) => ({
            id: socket.socketId,
            siteId: site.siteId,
            state: site.siteStatus,
            result: socket.programmingResult
          }));
          combined.push(...flatSockets);
        }
      }
    }

    setSocketsFn(combined);
  } catch (err) {
    console.error("Failed to load sockets for both sites", err);
  }
};

function App() {
  const [sockets, setSockets] = useState([]);
  const [jobRunning, setJobRunning] = useState(false);
  const [robotEnabled, setRobotEnabled] = useState(true);

  const loadJobStatus = async () => {
    try {
      const { data } = await getJobStatus();
      const jobIsRunning = data.data.jobStatus === "RUNNING";
      setJobRunning(jobIsRunning);
      if (jobIsRunning) {
        await fetchAndSetAllSockets(setSockets);
      }
    } catch (err) {
      console.error("Failed to get job status", err);
    }
  };

  const handleStart = async (qty) => {
    await startJob(qty);
    await loadJobStatus();
    await fetchAndSetAllSockets(setSockets);
  };

  useEffect(() => {
    const robotInterval = setInterval(async () => {
      if (!jobRunning || !robotEnabled) return;

      try {
        try {
          const placeResp = await getReadyToPlace();
          const placeSockets = Array.isArray(placeResp.data.data) ? placeResp.data.data : [];
          for (const socketId of placeSockets) {
            await placeDevice(socketId);
          }
        } catch (err) {
          if (err.response?.status !== 404) {
            console.error("Place operation failed", err.message || err);
          }
        }

        try {
          const pickResp = await getReadyToPick();
          const pickSockets = Array.isArray(pickResp.data.data) ? pickResp.data.data : [];
          for (const socketId of pickSockets) {
            await pickDevice(socketId);
          }
        } catch (err) {
          if (err.response?.status !== 404) {
            console.error("Pick operation failed", err.message || err);
          }
        }

      } catch (err) {
        console.error("Unexpected robot error", err.message || err);
      }
    }, 3000);

    return () => clearInterval(robotInterval);
  }, [jobRunning, robotEnabled]);

  useEffect(() => {
    const interval = setInterval(async () => {
      await loadJobStatus();
    }, 2000);
    return () => clearInterval(interval);
  }, []);

  return (
    <Container>
      <Typography variant="h4" gutterBottom>Programming System Integration Dashboard</Typography>

      <JobControlPanel
        onStart={handleStart}
        onPause={pauseJob}
        onResume={resumeJob}
        onStop={async () => {
          await stopJob();
          await loadJobStatus();
        }}
        robotEnabled={robotEnabled}
        setRobotEnabled={setRobotEnabled}
      />

      {!jobRunning && (
        <Alert severity="info" sx={{ mb: 2 }}>
          No job is currently running.
        </Alert>
      )}

      <SocketGrid sockets={sockets} />
      <StatsDisplay sockets={sockets} />
    </Container>
  );
}

export default App;
