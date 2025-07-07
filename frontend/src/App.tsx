import React, { useEffect, useState, useCallback } from 'react';
import { Container, Typography, Alert } from '@mui/material';
import {
  startJob, pauseJob, resumeJob, stopJob, getSockets, getJobStatus,
  getReadyToPick, getReadyToPlace, placeDevice, pickDevice
} from './services/api.ts';
import JobControlPanel from './components/JobControlPanel';
import SocketGrid from './components/SocketGrid';
import StatsDisplay from './components/StatsDisplay';
import { defaultJobStatus, JobState } from './models/Job.ts';
import { JobStatus } from './models/Job.ts';
import { Socket } from './models/Socket.ts';
import { useDevicePlacer } from './hooks/useDevicePlacer.ts';
import { useDevicePicker } from './hooks/useDevicePicker.ts';
import { useApiWrapper } from './hooks/useApiWrapper.ts';

function App() {
  const [sockets, setSockets] = useState<Socket[]>([]);
  const [jobRunning, setJobRunning] = useState(false);
  const [robotEnabled, setRobotEnabled] = useState(true);
  const [jobStats, setJobStats] = useState<JobStatus>(defaultJobStatus);

  const apiWrapper = useApiWrapper();

  const fetchAndSetAllSockets = useCallback(async () => {
    try {
      const [site1Res, site2Res] = await Promise.all([
        apiWrapper(() => getSockets(0), {
          context: 'getSockets(0)',
          notifySuccess: false,
          notifyFailure: false,
        }),
        apiWrapper(() => getSockets(1), {
          context: 'getSockets(1)',
          notifySuccess: false,
          notifyFailure: false,
        }),
      ]);

      const combined: Socket[] = [];

      for (const res of [site1Res, site2Res]) {
        if (!res) continue;
        const site = res.data.data?.[0];
        if (site?.sockets?.length) {
          const sockets = site.sockets.map((socket) => ({
            id: socket.socketId,
            siteId: site.siteId,
            state: site.siteStatus,
            result: socket.programmingResult,
            isSocketPlaced: socket.isSocketPlaced,
            isSocketPicked: socket.isSocketPicked
          }));
          combined.push(...sockets);
        }
      }

      setSockets(combined);
    } catch (err) {
      console.error("Failed to load sockets for both sites", err);
    }
  }, []);

  const loadJobStatus = useCallback(async () => {
    try {
      const res = await apiWrapper(() => getJobStatus(), {
        context: 'getJobStatus',
        notifySuccess: false,
      });

      if (!res || !res.data?.data) {
        console.warn("Job status response is undefined or empty.");
        return;
      }

      const job = res.data.data;

      setJobStats({
        jobStatus: job.jobStatus as JobState,
        requiredQuantity: job.requiredQuantity || 0,
        completedQuantity: job.completedQuantity || 0,
        passedQuantity: job.passedQuantity || 0,
        failedQuantity: job.failedQuantity || 0
      });

      setJobRunning(job.jobStatus === JobState.RUNNING);

      if (job.jobStatus === JobState.RUNNING) {
        await fetchAndSetAllSockets();
      }
    } catch (err) {
      console.error("Failed to get job status", err);
    }
  }, [fetchAndSetAllSockets]);

  const handleStart = async (qty: number) => {
    await apiWrapper(() => startJob(qty), {
      context: 'startJob',
      notifySuccess: true,
      notifyFailure: true
    });
    await loadJobStatus();
    await fetchAndSetAllSockets();
  };

  const handlePause = async () => {
    try {
      await apiWrapper(() => pauseJob(), {
        context: 'pauseJob',
        notifySuccess: true,
        notifyFailure: true,
      });
      await loadJobStatus();
    } catch (err) {
      console.error("Failed to pause job", err);
    }
  };

  const handleResume = async () => {
    try {
      await apiWrapper(() => resumeJob(), {
        context: 'resumeJob',
        notifySuccess: true,
        notifyFailure: true,
      });
      await loadJobStatus();
      await fetchAndSetAllSockets();
    } catch (err) {
      console.error("Failed to resume job", err);
    }
  };

  const handleStop = async () => {
    try {
      await apiWrapper(() => stopJob(), {
        context: 'stopJob',
        notifySuccess: true,
        notifyFailure: true,
      });
      await loadJobStatus();
      await fetchAndSetAllSockets();
    } catch (err) {
      console.error("Failed to stop job", err);
    }
  };

  useEffect(() => {
    fetchAndSetAllSockets();
    loadJobStatus();
  }, [fetchAndSetAllSockets, loadJobStatus]);

  useEffect(() => {
    if (!jobRunning) return;

    const interval = setInterval(() => {
      loadJobStatus();
    }, 2000);

    return () => clearInterval(interval);
  }, [jobRunning, loadJobStatus]);

  useDevicePlacer({
    jobRunning,
    robotEnabled,
    fetchSockets: () => fetchAndSetAllSockets(),
    getReadyToPlace,
    placeDevice,
  });

  useDevicePicker({
    jobRunning,
    robotEnabled,
    fetchSockets: () => fetchAndSetAllSockets(),
    getReadyToPick,
    pickDevice,
  });

  return (
    <Container>
      <Typography variant="h4" gutterBottom>Programming System Integration Dashboard</Typography>

      <JobControlPanel
        onStart={handleStart}
        onPause={handlePause}
        onResume={handleResume}
        onStop={handleStop}
        robotEnabled={robotEnabled}
        setRobotEnabled={setRobotEnabled}
        jobStatus={jobStats.jobStatus}
      />

      {jobStats.jobStatus === JobState.COMPLETED && (
        <Alert severity="success" sx={{ mb: 2 }}>
          🎉 Job has been completed successfully!
        </Alert>
      )}

      {jobStats.jobStatus === JobState.RUNNING && (
        <Alert severity="success" sx={{ mb: 2 }}>
          Job is running. Current status: {jobStats.jobStatus}
        </Alert>
      )}

      {jobStats.jobStatus !== JobState.RUNNING && jobStats.jobStatus !== JobState.COMPLETED && (
        <Alert severity="info" sx={{ mb: 2 }}>
          No job is currently running.
        </Alert>
      )}

      <SocketGrid sockets={sockets} />
      <StatsDisplay stats={jobStats} />

    </Container>
  );
}

export default App;
