import React from 'react';
import { Grid, Paper, Typography, Box } from '@mui/material';
import { ProgrammingResult, Socket } from '../models/Socket';

interface SiteGroup {
    siteStatus: keyof typeof siteStatusColors;
    sockets: Socket[];
}

interface Props {
    sockets: Socket[];
}

const siteStatusColors: Record<string, string> = {
    IDLE: '#f5f5f5',
    ACTIVE: '#fff3e0',
    COMPLETED: '#e8f5e9',
};

const getSocketColor = (result: ProgrammingResult): string => {
    switch (result) {
        case ProgrammingResult.PASSED:
            return '#c8e6c9';
        case ProgrammingResult.FAILED:
            return '#ffcdd2';
        case ProgrammingResult.NONE:
            return '#eeeeee';
        default:
            return '#f0f0f0';
    }
};

const getSocketBorder = (socket: Socket): string => {
    if (socket.isSocketPicked) return '3px solid #4caf50';
    if (socket.isSocketPlaced) return '3px dashed #ff9800';
    return '1px solid #ccc';
};

const SocketGrid: React.FC<Props> = ({ sockets }) => {
    const groupedBySite: Record<number, SiteGroup> = sockets.reduce((acc: Record<number, SiteGroup>, socket: Socket) => {
        if (!acc[socket.siteId]) {
            acc[socket.siteId] = {
                siteStatus: socket.state as keyof typeof siteStatusColors,
                sockets: [],
            };
        }
        acc[socket.siteId].sockets.push(socket);
        return acc;
    }, {});

    return (
        <Box>
            {/* Legend */}
            <Box
                mb={3}
                p={2}
                borderRadius={2}
                bgcolor="#e3f2fd"
                border="1px solid #90caf9"
            >
                <Typography variant="subtitle2" fontWeight="bold" mb={1}>
                    Socket Legend
                </Typography>

                <Box mt={2}>
                    <Typography variant="body2" fontWeight="bold"> Site Status</Typography>
                    <Box display="flex" gap={2} flexWrap="wrap" mt={1}>
                        <Box p={1} borderRadius={1} bgcolor="#f5f5f5" border="2px solid #888">
                            <Typography variant="body2"> IDLE → Light gray background</Typography>
                        </Box>
                        <Box p={1} borderRadius={1} bgcolor="#fff3e0" border="2px solid #888">
                            <Typography variant="body2"> ACTIVE → Soft orange background</Typography>
                        </Box>
                        <Box p={1} borderRadius={1} bgcolor="#e8f5e9" border="2px solid #888">
                            <Typography variant="body2"> COMPLETED → Light green background</Typography>
                        </Box>
                    </Box>
                </Box>

                <Box mb={2}>
                    <Typography variant="body2" fontWeight="bold"> Device Operations</Typography>
                    <Box display="flex" gap={2} flexWrap="wrap" mt={1}>
                        <Box p={1} borderRadius={1} bgcolor="white" border="2px solid #888">
                            <Typography variant="body2">
                                Picked → <span style={{ border: '3px solid #4caf50', padding: '2px 6px' }}>Green border</span>
                            </Typography>
                        </Box>
                        <Box p={1} borderRadius={1} bgcolor="white" border="2px solid #888">
                            <Typography variant="body2">
                                Placed → <span style={{ border: '3px dashed #ff9800', padding: '2px 6px' }}>Orange dashed border</span>
                            </Typography>
                        </Box>
                    </Box>
                </Box>

                <Box>
                    <Typography variant="body2" fontWeight="bold"> Programming Results</Typography>
                    <Box display="flex" gap={2} flexWrap="wrap" mt={1}>
                        <Box p={1} borderRadius={1} bgcolor="white" border="2px solid #888">
                            <Typography variant="body2">
                                Passed → <span style={{ backgroundColor: '#c8e6c9', padding: '2px 6px' }}>Green background</span>
                            </Typography>
                        </Box>
                        <Box p={1} borderRadius={1} bgcolor="white" border="2px solid #888">
                            <Typography variant="body2">
                                Failed → <span style={{ backgroundColor: '#ffcdd2', padding: '2px 6px' }}>Red background</span>
                            </Typography>
                        </Box>
                        <Box p={1} borderRadius={1} bgcolor="white" border="2px solid #888">
                            <Typography variant="body2">
                                None → <span style={{ backgroundColor: '#eeeeee', padding: '2px 6px' }}>Gray background</span>
                            </Typography>
                        </Box>
                    </Box>
                </Box>
            </Box>

            {/* Grid by site */}
            {Object.entries(groupedBySite).map(([siteId, siteGroup]) => (
                <Box
                    key={siteId}
                    mb={3}
                    p={2}
                    borderRadius={2}
                    style={{
                        backgroundColor: siteStatusColors[siteGroup.siteStatus] || '#f5f5f5',
                        border: '1px solid #bbb',
                    }}
                >
                    <Typography variant="h6" gutterBottom>
                        Site {siteId} ({siteGroup.siteStatus})
                    </Typography>
                    {/* <Grid container spacing={2}>
                        {siteGroup.sockets.map((socket, index) => (
                            <Grid item xs={3 as const} key={`${socket.siteId}-${socket.id}`}>
                                <Paper
                                    elevation={3}
                                    style={{
                                        padding: '1rem',
                                        backgroundColor: getSocketColor(socket.result),
                                        textAlign: 'center',
                                        border: getSocketBorder(socket),
                                    }}
                                >
                                    <Typography variant="subtitle2">
                                        #{index} | Socket {socket.id}
                                    </Typography>
                                    <Typography variant="body2">Result: {socket.result || 'NONE'}</Typography>
                                    <Typography variant="body2">
                                        Placed: {socket.isSocketPlaced ? 'Yes' : 'No'}
                                    </Typography>
                                    <Typography variant="body2">
                                        Picked: {socket.isSocketPicked ? 'Yes' : 'No'}
                                    </Typography>
                                </Paper>
                            </Grid>
                        ))}
                    </Grid> */}

                    <Box display="flex" flexWrap="wrap" gap={2}>
                        {siteGroup.sockets.map((socket, index) => (
                            <Box
                                key={`${socket.siteId}-${socket.id}`}
                                flexBasis="23%" 
                                minWidth="200px"
                            >
                                <Paper
                                    elevation={3}
                                    style={{
                                        padding: '1rem',
                                        backgroundColor: getSocketColor(socket.result),
                                        textAlign: 'center',
                                        border: getSocketBorder(socket),
                                    }}
                                >
                                    <Typography variant="subtitle2">
                                        #{index} | Socket {socket.id}
                                    </Typography>
                                    <Typography variant="body2">Result: {socket.result || 'NONE'}</Typography>
                                    <Typography variant="body2">
                                        Placed: {socket.isSocketPlaced ? 'Yes' : 'No'}
                                    </Typography>
                                    <Typography variant="body2">
                                        Picked: {socket.isSocketPicked ? 'Yes' : 'No'}
                                    </Typography>
                                </Paper>
                            </Box>
                        ))}
                    </Box>
                </Box>
            ))}
        </Box>
    );
};

export default SocketGrid;
