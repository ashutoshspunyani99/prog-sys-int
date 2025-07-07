import React from 'react';
import { render, screen, waitFor } from '@testing-library/react';
import App from './App';
import { SnackbarProvider } from 'contexts/SnackbarProvider';

jest.mock('./services/api', () => ({
  getJobStatus: jest.fn(() => Promise.resolve({
    data: {
      message: 'Fetched job status',
      data: {
        jobStatus: 'IDLE',
        requiredQuantity: 10,
        completedQuantity: 0,
        passedQuantity: 0,
        failedQuantity: 0
      }
    }
  })),
  getSockets: jest.fn(() => Promise.resolve({
    data: {
      message: 'Fetched sockets',
      data: [{
        siteId: 0,
        siteStatus: 'IDLE',
        sockets: []
      }]
    }
  })),
}));


const WrappedApp = (
  <SnackbarProvider>
    <App />
  </SnackbarProvider>
);


test('renders dashboard heading', async () => {
  render(WrappedApp);
  const heading = await screen.findByText(/Programming System Integration Dashboard/i);
  expect(heading).toBeInTheDocument();
});

test('shows "No job is currently running" alert by default', async () => {
  render(WrappedApp);
  const alert = await screen.findByText(/no job is currently running/i);
  expect(alert).toBeInTheDocument();
});
