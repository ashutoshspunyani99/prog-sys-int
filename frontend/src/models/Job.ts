export enum JobState {
    IDLE = 'IDLE',
    RUNNING = 'RUNNING',
    PAUSED = 'PAUSED',
    COMPLETED = 'COMPLETED'
  }

export interface JobStatus {
    jobStatus: JobState;
    requiredQuantity: number;
    completedQuantity: number;
    passedQuantity: number;
    failedQuantity: number;
}
  
export const defaultJobStatus:JobStatus = {
    jobStatus: JobState.IDLE,
    requiredQuantity: 0,
    completedQuantity: 0,
    passedQuantity: 0,
    failedQuantity: 0
  };