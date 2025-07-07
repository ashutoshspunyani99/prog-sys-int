import React, { useEffect, useState, useCallback } from 'react';
import { Container, Typography, Alert } from '@mui/material';
import {
  startJob,
  pauseJob,
  resumeJob,
  stopJob,
  getSockets,
  getJobStatus,
  getReadyToPick,
  getReadyToPlace,
  placeDevice,
  pickDevice,
} from './services/api';
import JobControlPanel from './components/JobControlPanel';
import SocketGrid from './components/SocketGrid';
import StatsDisplay from './components/StatsDisplay';
import { defaultJobStatus, JobState, JobStatus } from './models/Job';
import { Socket } from './models/Socket';
import { useDevicePlacer } from './hooks/useDevicePlacer';
import { useDevicePicker } from './hooks/useDevicePicker';
import { useApiWrapper } from './hooks/useApiWrapper';

const App: React.FC = () => {
  const [sockets, setSockets] = useState<Socket[]>([]);
  const [jobRunning, setJobRunning] = useState<boolean>(false);
  const [robotEnabled, setRobotEnabled] = useState<boolean>(true);
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
          const sockets: Socket[] = site.sockets.map((socket: any) => ({
            id: socket.socketId,
            siteId: site.siteId,
            state: site.siteStatus,
            result: socket.programmingResult,
            isSocketPlaced: socket.isSocketPlaced,
            isSocketPicked: socket.isSocketPicked,
          }));
          combined.push(...sockets);
        }
      }

      setSockets(combined);
    } catch (err) {
      console.error('[App] Failed to fetch sockets from both sites:', err);
    }
  }, []);

  const loadJobStatus = useCallback(async () => {
    try {
      const res = await apiWrapper(() => getJobStatus(), {
        context: 'getJobStatus',
        notifySuccess: false,
      });
      
      if (!res || !res.data?.data) {
        console.warn('[App] Job status unavailable or empty.');
        return;
      }

      const job = res.data.data;

      setJobStats({
        jobStatus: job.jobStatus as JobState,
        requiredQuantity: job.requiredQuantity || 0,
        completedQuantity: job.completedQuantity || 0,
        passedQuantity: job.passedQuantity || 0,
        failedQuantity: job.failedQuantity || 0,
      });
      setJobRunning(job.jobStatus === JobState.RUNNING);

      if (job.jobStatus === JobState.RUNNING) {
        await fetchAndSetAllSockets();
      }
    } catch (err) {
      console.error('[App] Failed to fetch job status:', err);
    }
  }, [fetchAndSetAllSockets]);

  const handleStart = async (qty: number) => {
    await apiWrapper(() => startJob(qty), {
      context: 'startJob',
      notifySuccess: true,
      notifyFailure: true,
    });
    await loadJobStatus();
    await fetchAndSetAllSockets();
  };

  const handlePause = async () => {
    await apiWrapper(() => pauseJob(), {
      context: 'pauseJob',
      notifySuccess: true,
      notifyFailure: true,
    });
    await loadJobStatus();
  };

  const handleResume = async () => {
    await apiWrapper(() => resumeJob(), {
      context: 'resumeJob',
      notifySuccess: true,
      notifyFailure: true,
    });
    await loadJobStatus();
    await fetchAndSetAllSockets();
  };

  const handleStop = async () => {
    await apiWrapper(() => stopJob(), {
      context: 'stopJob',
      notifySuccess: true,
      notifyFailure: true,
    });
    await loadJobStatus();
    await fetchAndSetAllSockets();
  };

  useEffect(() => {
    const init = async () => {
      await fetchAndSetAllSockets();
    };
    init();
  }, []);

  useEffect(() => {
    if (!jobRunning) return;

    const interval = setInterval(() => {
      loadJobStatus();
    }, 2000);

    return () => clearInterval(interval);
  }, [jobRunning]);

  useDevicePlacer({
    jobRunning,
    robotEnabled,
    fetchSockets: fetchAndSetAllSockets,
    getReadyToPlace,
    placeDevice,
  });

  useDevicePicker({
    jobRunning,
    robotEnabled,
    fetchSockets: fetchAndSetAllSockets,
    getReadyToPick,
    pickDevice,
  });

  return (
    <Container>
      <Typography variant="h4" gutterBottom>
        Programming System Integration Dashboard
      </Typography>

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

      {jobStats.jobStatus !== JobState.RUNNING &&
        jobStats.jobStatus !== JobState.COMPLETED && (
          <Alert severity="info" sx={{ mb: 2 }}>
            No job is currently running.
          </Alert>
        )}

      <SocketGrid sockets={sockets} />
      <StatsDisplay stats={jobStats} />
    </Container>
  );
};

export default App;
