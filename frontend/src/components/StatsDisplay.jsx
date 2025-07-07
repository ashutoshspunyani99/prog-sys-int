import React from 'react';
import { Typography, Box, LinearProgress } from '@mui/material';

const StatsDisplay = ({ stats }) => {
    const {
        jobStatus,
        requiredQuantity,
        completedQuantity,
        passedQuantity,
        failedQuantity
    } = stats;

    const progress = requiredQuantity > 0
        ? (completedQuantity / requiredQuantity) * 100
        : 0;

    return (
        <Box mt={2}>
            <Typography variant="body1">Job Status: {jobStatus}</Typography>
            <Typography variant="body1">Required Quantity: {requiredQuantity}</Typography>
            <Typography variant="body1">Completed Quantity: {completedQuantity}</Typography>
            <Typography variant="body1">Passed: {passedQuantity}</Typography>
            <Typography variant="body1">Failed: {failedQuantity}</Typography>
            <Box mt={1}>
                <LinearProgress variant="determinate" value={progress} />
                <Typography variant="caption">{Math.round(progress)}% Complete</Typography>
            </Box>
        </Box>
    );
};

export default StatsDisplay;
