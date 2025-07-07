export enum ProgrammingResult {
  PASSED = 'PASSED',
  FAILED = 'FAILED',
  NONE = 'NONE'
}

export interface Socket {
  id: number;
  siteId: number;
  state: string;
  result: ProgrammingResult;
  isSocketPlaced: boolean;
  isSocketPicked: boolean;
}