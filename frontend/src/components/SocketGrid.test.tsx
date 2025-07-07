import React from 'react';
import { render, screen } from '@testing-library/react';
import SocketGrid from './SocketGrid';
import { ProgrammingResult } from '../models/Socket';

describe('SocketGrid', () => {
    const mockSockets = [
        {
            id: 1,
            siteId: 0,
            state: 'ACTIVE',
            result: ProgrammingResult.PASSED,
            isSocketPicked: true,
            isSocketPlaced: false,
        },
        {
            id: 2,
            siteId: 0,
            state: 'ACTIVE',
            result: ProgrammingResult.FAILED,
            isSocketPicked: false,
            isSocketPlaced: true,
        },
        {
            id: 3,
            siteId: 1,
            state: 'IDLE',
            result: ProgrammingResult.NONE,
            isSocketPicked: false,
            isSocketPlaced: false,
        },
    ];

    test('renders socket site titles and results correctly', () => {
        render(<SocketGrid sockets={mockSockets} />);

        expect(screen.getByText(/Site 0/i)).toBeInTheDocument();
        expect(screen.getByText(/Site 1/i)).toBeInTheDocument();

        expect(screen.getByText(/Socket 1/)).toBeInTheDocument();
        expect(screen.getByText(/Result: PASSED/)).toBeInTheDocument();

        expect(screen.getByText(/Socket 2/)).toBeInTheDocument();
        expect(screen.getByText(/Result: FAILED/)).toBeInTheDocument();

        expect(screen.getByText(/Socket 3/)).toBeInTheDocument();
        expect(screen.getByText(/Result: NONE/)).toBeInTheDocument();
    });
    test('displays "Picked: Yes" and "Placed: Yes" correctly', () => {
        render(<SocketGrid sockets={mockSockets} />);

        expect(screen.getAllByText(/Picked: Yes/)).toHaveLength(1);
        expect(screen.getAllByText(/Picked: No/)).toHaveLength(2);

        expect(screen.getAllByText(/Placed: Yes/)).toHaveLength(1);
        expect(screen.getAllByText(/Placed: No/)).toHaveLength(2);
    });

    test('renders socket legend sections', () => {
        render(<SocketGrid sockets={mockSockets} />);

        expect(screen.getByText(/Socket Legend/i)).toBeInTheDocument();
        expect(screen.getByText(/Site Status/i)).toBeInTheDocument();
        expect(screen.getByText(/Device Operations/i)).toBeInTheDocument();
        expect(screen.getByText(/Programming Results/i)).toBeInTheDocument();
    });

    test('groups sockets by siteId correctly', () => {
        render(<SocketGrid sockets={mockSockets} />);

        const site0Title = screen.getByText(/Site 0/i);
        const site1Title = screen.getByText(/Site 1/i);

        expect(site0Title).toBeInTheDocument();
        expect(site1Title).toBeInTheDocument();

        expect(screen.getByText(/Socket 1/)).toBeInTheDocument();
        expect(screen.getByText(/Socket 2/)).toBeInTheDocument();
        expect(screen.getByText(/Socket 3/)).toBeInTheDocument();
    });

    test('applies correct background for site status', () => {
        render(<SocketGrid sockets={mockSockets} />);

        const site0 = screen.getByText(/Site 0/).closest('div');
        const site1 = screen.getByText(/Site 1/).closest('div');

        expect(site0).toHaveStyle('background-color: #fff3e0'); 
        expect(site1).toHaveStyle('background-color: #f5f5f5'); 
    });
});
