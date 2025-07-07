import { useEffect } from 'react';
import { useApiWrapper } from './useApiWrapper.ts'; 

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
                    // const resp = await getReadyToPlace();
                    const resp = await apiWrapper(() => getReadyToPlace(), {
                        context: 'getReadyToPlace',
                        // notifySuccess: true,
                        notifyFailure: true,
                      });
                      
                    const placeSockets = Array.isArray(resp?.data?.data) ? resp?.data?.data : [];

                    for (const socketId of placeSockets) {
                        await new Promise((res) => setTimeout(res, 1000)); 
                        // await placeDevice(socketId);
                        await apiWrapper(() => placeDevice(socketId), {
                            context: 'placeDevice',
                            notifySuccess: true,
                            notifyFailure: true,
                        });
                        console.log(`Placed device at socket ${socketId}`);
                        await fetchSockets();
                    }
                } catch (err) {
                    if (err.response?.status !== 404) {
                        console.error("Place error:", err.message || err);
                    }
                }
            };

            runPlacement(); 
        }, 5000);

        return () => clearInterval(interval);
    }, [jobRunning, robotEnabled]);
    
};
