import { useEffect } from 'react';
import { useApiWrapper } from './useApiWrapper.ts';

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
                    // const resp = await getReadyToPick();
                    const resp = await apiWrapper(() => getReadyToPick(), {
                        context: 'getReadyToPick',
                        // notifySuccess: true,
                        notifyFailure: true,
                    });
                    const pickSockets = Array.isArray(resp?.data?.data) ? resp?.data?.data : [];

                    for (const socketId of pickSockets) {
                        await new Promise((res) => setTimeout(res, 1000)); // fixed 1s delay

                        // await pickDevice(socketId);
                        await apiWrapper(() => pickDevice(socketId), {
                            context: 'pickDevice',
                            notifySuccess: true,
                            notifyFailure: true,
                        });
                        console.log(`Picked device from socket ${socketId}`);
                        await fetchSockets();
                    }
                } catch (err) {
                    if (err.response?.status !== 404) {
                        console.error("Pick error:", err.message || err);
                    }
                }
            };

            runPicking();
        }, 5000);

        return () => clearInterval(interval);
    }, [jobRunning, robotEnabled]);
};
