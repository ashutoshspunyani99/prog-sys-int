import axios from 'axios';
import { ApiResponse } from '../types/ApiResponse.ts';

const API_BASE = 'http://localhost:8000/api';

export const startJob = (quantity: number) =>
    axios.post<ApiResponse<any>>(`${API_BASE}/job/start`, { requiredQuantity: quantity });

export const pauseJob = () =>
    axios.post<ApiResponse<any>>(`${API_BASE}/job/pause`);

export const resumeJob = () =>
    axios.post<ApiResponse<any>>(`${API_BASE}/job/resume`);

export const stopJob = () =>
    axios.post<ApiResponse<any>>(`${API_BASE}/job/stop`);

export const getSockets = (siteId: number) =>
    axios.get<ApiResponse<any>>(`${API_BASE}/programming/sockets`, {
        params: { siteId },
    });

export const getReadyToPick = () =>
    axios.get<ApiResponse<any>>(`${API_BASE}/robot/ready-to-pick`);

export const getReadyToPlace = () =>
    axios.get<ApiResponse<any>>(`${API_BASE}/robot/ready-to-place`);

export const placeDevice = (socketId: number) =>
    axios.post<ApiResponse<any>>(`${API_BASE}/robot/device-placed?socketId=${socketId}`);

export const pickDevice = (socketId: number) =>
    axios.post<ApiResponse<any>>(`${API_BASE}/robot/device-picked?socketId=${socketId}`);

export const getJobStatus = () =>
    axios.get<ApiResponse<any>>(`${API_BASE}/job/status`);