const express = require('express');
const app = express();
const rotaMedicao = require('./routes/medicao');
const port = 3000;

app.use(express.json());
app.use('/medicao', rotaMedicao);
app.listen(port, () => console.log(`Server running on http://localhost:${port}`));

// oi