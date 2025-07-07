import React, { useState } from 'react';
import { Button, TextField, Box, FormControlLabel, Switch } from '@mui/material';

const JobControlPanel = ({
    onStart,
    onPause,
    onResume,
    onStop,
    robotEnabled,
    setRobotEnabled,
    jobStatus
}) => {
    const [quantity, setQuantity] = useState('');

    return (
        <Box display="flex" flexWrap="wrap" gap={2} mb={2} alignItems="center">
            <TextField
                label="Quantity"
                type="number"
                value={quantity}
                onChange={(e) => setQuantity(e.target.value)}
                disabled={jobStatus === 'RUNNING' || jobStatus === 'PAUSED'}
            />

            <Button
                variant="contained"
                onClick={() => onStart(Number(quantity))}
                disabled={jobStatus === 'RUNNING' || jobStatus === 'PAUSED'}
            >
                Start
            </Button>

            <Button
                variant="outlined"
                onClick={onPause}
                disabled={jobStatus !== 'RUNNING'}
            >
                Pause
            </Button>

            <Button
                variant="outlined"
                onClick={onResume}
                disabled={jobStatus !== 'PAUSED'}
            >
                Resume
            </Button>

            <Button
                variant="contained"
                color="error"
                onClick={onStop}
                disabled={jobStatus === 'IDLE' || jobStatus === 'COMPLETED'}
            >
                Stop
            </Button>

            <FormControlLabel
                control={
                    <Switch
                        checked={robotEnabled}
                        onChange={(e) => setRobotEnabled(e.target.checked)}
                        color="primary"
                    />
                }
                label="Auto Robot"
            />
        </Box>
    );
};

export default JobControlPanel;
