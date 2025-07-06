import React from 'react';
import { Typography, Box } from '@mui/material';

const StatsDisplay = ({ sockets }) => {
    const passed = sockets.filter(s => s.result === 'PASSED').length;
    const failed = sockets.filter(s => s.result === 'FAILED').length;
    const total = passed + failed;

    return (
        <Box mt={2}>
            <Typography variant="body1">Total Completed: {total}</Typography>
            <Typography variant="body1">Passed: {passed}</Typography>
            <Typography variant="body1">Failed: {failed}</Typography>
        </Box>
    );
};

export default StatsDisplay;
