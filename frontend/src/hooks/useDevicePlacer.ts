import { useEffect } from 'react';
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
    useEffect(() => {
        console.log("Device Placer Hook initialized");

        const interval = setInterval(() => {
            if (!jobRunning || !robotEnabled) return;

            const runPlacement = async () => {
                console.log("runPlacement triggered");
                try {
                    console.log("Checking for devices to place...");
                    const resp = await apiWrapper(() => getReadyToPlace(), {
                        context: 'getReadyToPlace',
                        notifyFailure: true,
                    });

                    const placeSockets: number[] = Array.isArray(resp?.data?.data) ? resp!.data!.data : [];

                    for (const socketId of placeSockets) {
                        await new Promise((res) => setTimeout(res, 3000));
                        await apiWrapper(() => placeDevice(socketId), {
                            context: 'placeDevice',
                            notifySuccess: true,
                        });
                        console.log(`Placed device at socket ${socketId}`);
                        await fetchSockets();
                    }
                } catch (err) {
                    if (
                        typeof err === 'object' &&
                        err !== null &&
                        'response' in err &&
                        (err as any).response?.status !== 404
                    ) {
                        console.error('Pick error:', (err as any).message || err);
                    }
                }
            };

            runPlacement();
        }, 5000);

        return () => clearInterval(interval);
    }, [jobRunning, robotEnabled]);

};
