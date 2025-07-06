import React, { useState } from 'react';
import { Button, TextField, Box, Switch, FormControlLabel } from '@mui/material';

const JobControlPanel = ({ onStart, onPause, onResume, onStop, robotEnabled, setRobotEnabled }) => {
    const [quantity, setQuantity] = useState('');

    return (
        <Box display="flex" alignItems="center" flexWrap="wrap" gap={2} mb={2}>
            <TextField
                label="Quantity"
                type="number"
                value={quantity}
                onChange={(e) => setQuantity(e.target.value)}
            />
            <Button variant="contained" onClick={() => onStart(Number(quantity))}>Start</Button>
            <Button variant="outlined" onClick={onPause}>Pause</Button>
            <Button variant="outlined" onClick={onResume}>Resume</Button>
            <Button variant="contained" color="error" onClick={onStop}>Stop</Button>

            <FormControlLabel
                control={
                    <Switch
                        checked={robotEnabled}
                        onChange={() => setRobotEnabled(prev => !prev)}
                    />
                }
                label="Auto Robot"
            />
        </Box>
    );
};

export default JobControlPanel;
