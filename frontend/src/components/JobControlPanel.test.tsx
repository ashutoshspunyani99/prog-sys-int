import { render, screen, fireEvent } from '@testing-library/react';
import JobControlPanel from './JobControlPanel';
import { JobState } from 'models/Job';

describe('JobControlPanel', () => {
    test('calls onStart with correct quantity', () => {
        const onStart = jest.fn();

        render(
            <JobControlPanel
                onStart={onStart}
                onPause={() => { }}
                onResume={() => { }}
                onStop={() => { }}
                robotEnabled={true}
                setRobotEnabled={() => { }}
                jobStatus={JobState.IDLE}
            />
        );

        const input = screen.getByLabelText(/Quantity/i);
        const button = screen.getByRole('button', { name: /start/i });

        fireEvent.change(input, { target: { value: '5' } });
        fireEvent.click(button);

        expect(onStart).toHaveBeenCalledWith(5);
    });
    test('pause button is disabled when job is not RUNNING', () => {
        render(
            <JobControlPanel
                onStart={() => { }}
                onPause={() => { }}
                onResume={() => { }}
                onStop={() => { }}
                robotEnabled={false}
                setRobotEnabled={() => { }}
                jobStatus={JobState.IDLE}
            />
        );

        const pauseButton = screen.getByRole('button', { name: /pause/i });
        expect(pauseButton).toBeDisabled();
    });
    test('resume button is enabled only when job is PAUSED', () => {
        const { rerender } = render(
            <JobControlPanel
                onStart={() => { }}
                onPause={() => { }}
                onResume={() => { }}
                onStop={() => { }}
                robotEnabled={false}
                setRobotEnabled={() => { }}
                jobStatus={JobState.RUNNING}
            />
        );

        expect(screen.getByRole('button', { name: /resume/i })).toBeDisabled();

        rerender(
            <JobControlPanel
                onStart={() => { }}
                onPause={() => { }}
                onResume={() => { }}
                onStop={() => { }}
                robotEnabled={false}
                setRobotEnabled={() => { }}
                jobStatus={JobState.PAUSED}
            />
        );

        expect(screen.getByRole('button', { name: /resume/i })).toBeEnabled();
    });
    test('stop button is disabled when job is IDLE or COMPLETED', () => {
        const { rerender } = render(
            <JobControlPanel
                onStart={() => { }}
                onPause={() => { }}
                onResume={() => { }}
                onStop={() => { }}
                robotEnabled={false}
                setRobotEnabled={() => { }}
                jobStatus={JobState.IDLE}
            />
        );

        expect(screen.getByRole('button', { name: /stop/i })).toBeDisabled();

        rerender(
            <JobControlPanel
                onStart={() => { }}
                onPause={() => { }}
                onResume={() => { }}
                onStop={() => { }}
                robotEnabled={false}
                setRobotEnabled={() => { }}
                jobStatus={JobState.COMPLETED}
            />
        );

        expect(screen.getByRole('button', { name: /stop/i })).toBeDisabled();
    });
    test('robot switch toggles state', () => {
        const setRobotEnabled = jest.fn();

        render(
            <JobControlPanel
                onStart={() => { }}
                onPause={() => { }}
                onResume={() => { }}
                onStop={() => { }}
                robotEnabled={true}
                setRobotEnabled={setRobotEnabled}
                jobStatus={JobState.IDLE}
            />
        );

        const switchEl = screen.getByRole('checkbox', { name: /auto robot/i });
        fireEvent.click(switchEl);

        expect(setRobotEnabled).toHaveBeenCalledWith(false); 
    });
    test('start button is disabled when job is RUNNING or PAUSED', () => {
        const { rerender } = render(
            <JobControlPanel
                onStart={() => { }}
                onPause={() => { }}
                onResume={() => { }}
                onStop={() => { }}
                robotEnabled={true}
                setRobotEnabled={() => { }}
                jobStatus={JobState.RUNNING}
            />
        );

        expect(screen.getByRole('button', { name: /start/i })).toBeDisabled();

        rerender(
            <JobControlPanel
                onStart={() => { }}
                onPause={() => { }}
                onResume={() => { }}
                onStop={() => { }}
                robotEnabled={true}
                setRobotEnabled={() => { }}
                jobStatus={JobState.PAUSED}
            />
        );

        expect(screen.getByRole('button', { name: /start/i })).toBeDisabled();
    });
    test('quantity input is disabled during RUNNING or PAUSED', () => {
        const { rerender } = render(
            <JobControlPanel
                onStart={() => { }}
                onPause={() => { }}
                onResume={() => { }}
                onStop={() => { }}
                robotEnabled={true}
                setRobotEnabled={() => { }}
                jobStatus={JobState.RUNNING}
            />
        );

        expect(screen.getByLabelText(/quantity/i)).toBeDisabled();

        rerender(
            <JobControlPanel
                onStart={() => { }}
                onPause={() => { }}
                onResume={() => { }}
                onStop={() => { }}
                robotEnabled={true}
                setRobotEnabled={() => { }}
                jobStatus={JobState.PAUSED}
            />
        );

        expect(screen.getByLabelText(/quantity/i)).toBeDisabled();
    });
    test('does not call onStart when quantity is empty or not a number', () => {
        const onStart = jest.fn();

        render(
            <JobControlPanel
                onStart={onStart}
                onPause={() => { }}
                onResume={() => { }}
                onStop={() => { }}
                robotEnabled={true}
                setRobotEnabled={() => { }}
                jobStatus={JobState.IDLE}
            />
        );

        const input = screen.getByLabelText(/quantity/i);
        const startBtn = screen.getByRole('button', { name: /start/i });

        fireEvent.change(input, { target: { value: '' } });
        fireEvent.click(startBtn);
        expect(onStart).not.toHaveBeenCalledWith(NaN);

        fireEvent.change(input, { target: { value: 'abc' } });
        fireEvent.click(startBtn);
        expect(onStart).not.toHaveBeenCalledWith(NaN); 
    });
    test('pause, resume, stop buttons call respective handlers', () => {
        const onPause = jest.fn();
        const onResume = jest.fn();
        const onStop = jest.fn();

        const { rerender } = render(
            <JobControlPanel
                onStart={() => { }}
                onPause={onPause}
                onResume={onResume}
                onStop={onStop}
                robotEnabled={true}
                setRobotEnabled={() => { }}
                jobStatus={JobState.RUNNING}
            />
        );

        fireEvent.click(screen.getByRole('button', { name: /pause/i }));
        expect(onPause).toHaveBeenCalled();

        rerender(
            <JobControlPanel
                onStart={() => { }}
                onPause={onPause}
                onResume={onResume}
                onStop={onStop}
                robotEnabled={true}
                setRobotEnabled={() => { }}
                jobStatus={JobState.PAUSED}
            />
        );

        fireEvent.click(screen.getByRole('button', { name: /resume/i }));
        expect(onResume).toHaveBeenCalled();

        rerender(
            <JobControlPanel
                onStart={() => { }}
                onPause={onPause}
                onResume={onResume}
                onStop={onStop}
                robotEnabled={true}
                setRobotEnabled={() => { }}
                jobStatus={JobState.RUNNING}
            />
        );

        fireEvent.click(screen.getByRole('button', { name: /stop/i }));
        expect(onStop).toHaveBeenCalled();
    });
    test('robot switch label is visible and toggles state', () => {
        const setRobotEnabled = jest.fn();

        render(
            <JobControlPanel
                onStart={() => { }}
                onPause={() => { }}
                onResume={() => { }}
                onStop={() => { }}
                robotEnabled={true}
                setRobotEnabled={setRobotEnabled}
                jobStatus={JobState.IDLE}
            />
        );

        const switchEl = screen.getByRole('checkbox', { name: /auto robot/i });
        expect(switchEl).toBeChecked();

        fireEvent.click(switchEl);
        expect(setRobotEnabled).toHaveBeenCalledWith(false);
    });
});
