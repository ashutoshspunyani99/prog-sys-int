import { useEffect } from 'react';
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
    useEffect(() => {
        console.log("Device Picker Hook initialized");

        const interval = setInterval(() => {
            if (!jobRunning || !robotEnabled) return;

            const runPicking = async () => {
                console.log("runPicking triggered");
                try {
                    console.log("Checking for devices to pick...");
                    const resp = await apiWrapper(() => getReadyToPick(), {
                        context: 'getReadyToPick',
                        notifyFailure: true,
                    });

                    
                    const pickSockets: number[] = Array.isArray(resp?.data?.data) ? resp!.data!.data : [];

                    for (const socketId of pickSockets) {
                        await new Promise((res) => setTimeout(res, 3000)); 

                        await apiWrapper(() => pickDevice(socketId), {
                            context: 'pickDevice',
                            notifySuccess: true,
                        });
                        console.log(`Picked device from socket ${socketId}`);
                        await fetchSockets();
                    }
                } catch (err:unknown) {
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

            runPicking();
        }, 5000);

        return () => clearInterval(interval);
    }, [jobRunning, robotEnabled]);
};
