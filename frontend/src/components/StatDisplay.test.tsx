import React from 'react';
import { render, screen } from '@testing-library/react';
import StatsDisplay from './StatsDisplay';
import { JobState } from '../models/Job';

describe('StatsDisplay', () => {
    it('renders all job stats correctly', () => {
        const mockStats = {
            jobStatus: JobState.RUNNING,
            requiredQuantity: 100,
            completedQuantity: 40,
            passedQuantity: 35,
            failedQuantity: 5,
        };

        render(<StatsDisplay stats={mockStats} />);

        expect(screen.getByText(/Job Status: RUNNING/i)).toBeInTheDocument();
        expect(screen.getByText(/Required Quantity: 100/i)).toBeInTheDocument();
        expect(screen.getByText(/Completed Quantity: 40/i)).toBeInTheDocument();
        expect(screen.getByText(/Passed: 35/i)).toBeInTheDocument();
        expect(screen.getByText(/Failed: 5/i)).toBeInTheDocument();
        expect(screen.getByText(/40% Complete/i)).toBeInTheDocument();
    });

    it('shows 0% progress if required quantity is 0', () => {
        const mockStats = {
            jobStatus: JobState.IDLE,
            requiredQuantity: 0,
            completedQuantity: 0,
            passedQuantity: 0,
            failedQuantity: 0,
        };

        render(<StatsDisplay stats={mockStats} />);
        expect(screen.getByText(/0% Complete/i)).toBeInTheDocument();
    });
});
