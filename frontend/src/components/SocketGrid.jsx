import React from 'react';
import { Grid, Paper, Typography, Box } from '@mui/material';

const getColor = (status) => {
    if (status === 'ACTIVE') return '#1976d2';
    if (status === 'COMPLETED') return '#388e3c';
    return '#e0e0e0';
};

const SocketGrid = ({ sockets }) => {
    const groupedBySite = sockets.reduce((acc, socket) => {
        acc[socket.siteId] = acc[socket.siteId] || [];
        acc[socket.siteId].push(socket);
        return acc;
    }, {});

    return (
        <Box>
            {Object.entries(groupedBySite).map(([siteId, siteSockets]) => (
                <Box key={siteId} mb={3}>
                    <Typography variant="h6" gutterBottom>
                        Site {siteId}
                    </Typography>
                    <Grid container spacing={2}>
                        {siteSockets.map((socket) => (
                            <Grid item xs={3} key={`${socket.siteId}-${socket.id}`}>
                                <Paper
                                    elevation={3}
                                    style={{
                                        padding: '1rem',
                                        backgroundColor: getColor(socket.state),
                                        textAlign: 'center',
                                        border: '1px solid #ccc',
                                    }}
                                >
                                    <Typography variant="subtitle2">Socket {socket.id}</Typography>
                                    <Typography variant="body2">{socket.state}</Typography>
                                    <Typography variant="body2">
                                        {socket.result && socket.result !== 'NONE' ? socket.result : 'Pending'}
                                    </Typography>
                                </Paper>
                            </Grid>
                        ))}
                    </Grid>
                </Box>
            ))}
        </Box>
    );
};

export default SocketGrid;
