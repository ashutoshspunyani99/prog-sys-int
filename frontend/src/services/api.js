import axios from 'axios';

const API_BASE = 'http://localhost:8000/api';

export const startJob = (quantity) => axios.post(`${API_BASE}/job/start`, { requiredQuantity: quantity });
export const pauseJob = () => axios.post(`${API_BASE}/job/pause`);
export const resumeJob = () => axios.post(`${API_BASE}/job/resume`);
export const stopJob = () => axios.post(`${API_BASE}/job/stop`);
export const getSockets = (siteId) => axios.get(`${API_BASE}/programming/sockets`, {
    params: { siteId },
});
export const getReadyToPick = () => axios.get(`${API_BASE}/robot/ready-to-pick`);
export const getReadyToPlace = () => axios.get(`${API_BASE}/robot/ready-to-place`);
export const placeDevice = (socketId) => axios.post(`${API_BASE}/robot/device-placed?socketId=${socketId}`);
export const pickDevice = (socketId) => axios.post(`${API_BASE}/robot/device-picked?socketId=${socketId}`);
export const getJobStatus = () => axios.get(`${API_BASE}/job/status`);