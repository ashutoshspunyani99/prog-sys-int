import { useEffect, useRef } from 'react';
import { useApiWrapper } from './useApiWrapper';

export const useDevicePlacer = ({
    jobRunning,
    robotEnabled,
    fetchSockets,
    getReadyToPlace,
    placeDevice,
}: {
    jobRunning: boolean;
    robotEnabled: boolean;
    fetchSockets: () => Promise<void>;
    getReadyToPlace: () => Promise<any>;
    placeDevice: (socketId: number) => Promise<any>;
}) => {
    const apiWrapper = useApiWrapper();
    const placedSocketsRef = useRef<Set<number>>(new Set());
    useEffect(() => {
        console.info('[DevicePlacer] Polling started');


        const interval = setInterval(() => {
            if (!jobRunning || !robotEnabled) return;

            const runPlacement = async () => {
                try {
                    const resp = await apiWrapper(() => getReadyToPlace(), {
                        context: 'getReadyToPlace',
                        notifyFailure: true,
                    });

                    const placeSockets: number[] = Array.isArray(resp?.data?.data) ? resp!.data!.data : [];

                    for (const socketId of placeSockets) {
                        if (placedSocketsRef.current.has(socketId)) {
                            continue; 
                          }
                        placedSocketsRef.current.add(socketId);
                        setTimeout(() => placedSocketsRef.current.delete(socketId), 5000);
                        await new Promise((res) => setTimeout(res, 3000));
                        await apiWrapper(() => placeDevice(socketId), {
                            context: `placeDevice [Socket ${socketId}]`,
                            notifySuccess: true,
                        });
                        await fetchSockets();
                    }
                } catch (err) {
                    if (
                        typeof err === 'object' &&
                        err !== null &&
                        'response' in err &&
                        (err as any).response?.status !== 404
                    ) {
                        console.error('[DevicePlacer] Unexpected error:', err);
                    }
                }
            };

            runPlacement();
        }, 5000);

        return () => {
            console.info('[DevicePlacer] Polling stopped');
            clearInterval(interval);
            placedSocketsRef.current.clear();
        };
    }, [jobRunning, robotEnabled]);

};
