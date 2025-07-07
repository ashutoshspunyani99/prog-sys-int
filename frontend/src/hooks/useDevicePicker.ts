import { useEffect, useRef } from 'react';
import { useApiWrapper } from './useApiWrapper';

export const useDevicePicker = ({
    jobRunning,
    robotEnabled,
    fetchSockets,
    getReadyToPick,
    pickDevice,
}: {
    jobRunning: boolean;
    robotEnabled: boolean;
    fetchSockets: () => Promise<void>;
    getReadyToPick: () => Promise<any>;
    pickDevice: (socketId: number) => Promise<any>;
}) => {
    const apiWrapper = useApiWrapper();
    const pickedSocketsRef = useRef<Set<number>>(new Set());
    useEffect(() => {
        console.info('[DevicePicker] Polling started');

        const interval = setInterval(() => {
            if (!jobRunning || !robotEnabled) return;

            const runPicking = async () => {
                try {
                    const resp = await apiWrapper(() => getReadyToPick(), {
                        context: 'getReadyToPick',
                        notifyFailure: true,
                    });


                    const pickSockets: number[] = Array.isArray(resp?.data?.data) ? resp!.data!.data : [];

                    for (const socketId of pickSockets) {
                        if (pickedSocketsRef.current.has(socketId)) {
                            continue;
                          }

                        pickedSocketsRef.current.add(socketId);
                        setTimeout(() => pickedSocketsRef.current.delete(socketId), 5000);
                        await new Promise((res) => setTimeout(res, 3000));

                        await apiWrapper(() => pickDevice(socketId), {
                            context: `pickDevice [Socket ${socketId}]`,
                            notifySuccess: true,
                        });
                        await fetchSockets();
                    }
                } catch (err: unknown) {
                    if (
                        typeof err === 'object' &&
                        err !== null &&
                        'response' in err &&
                        (err as any).response?.status !== 404
                    ) {
                        console.error('[DevicePicker] Unexpected error:', err);
                    }
                }
            };

            runPicking();
        }, 5000);

        return () => {
            console.info('[DevicePicker] Polling stopped');
            clearInterval(interval);
            pickedSocketsRef.current.clear();
        };
    }, [jobRunning, robotEnabled]);
};
