const express = require('express');
const router = express.Router();
const sqlite3 = require('sqlite3').verbose();


// Create a SQLite database connection
const db = new sqlite3.Database('database.sqlite');

// Create a table for storing humidity data
db.serialize(() => {
  db.run(
    `CREATE TABLE IF NOT EXISTS medicao (
      id INTEGER PRIMARY KEY AUTOINCREMENT,
      umidade REAL,
      temperatura REAL,
      ninho REAL,
      timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
    )`
  );
});

// Endpoint for receiving humidity data
router.post('/', (req, res) => {
  const { umidade, temperatura, ninho } = req.body;

  // Insert humidity data into the database
  db.run('INSERT INTO medicao (umidade, temperatura, ninho) VALUES (?, ?, ?)', [umidade, temperatura, ninho], function(err) {
    if (err) {
      console.error(err.message);
      return res.status(500).send('Error inserting data.');
    }

    console.log(`Data added with ID: ${this.lastID}`);
    res.sendStatus(200);
  });
});


// Endpoint para exibir os dados em uma tabela HTML
router.get('/tabela', (req, res) => {
  // Consulta para selecionar todos os dados da tabela "medicao"
  const query = 'SELECT * FROM medicao';

  db.all(query, (err, rows) => {
    if (err) {
      console.error('Erro ao executar a consulta:', err);
      return res.status(500).send('Erro no Servidor Interno');
    }

    // Renderiza os dados em uma tabela HTML
    const htmlTable = `
      <html>
        <head>
          <title>Tabela de Medição</title>
          <style>
            /* Estilo para a tabela */
            table {
                border-collapse: collapse;
                width: 100%;
                border: 1px solid #ddd;
                font-family: Arial, sans-serif;
            }

            /* Estilo para as células de cabeçalho */
            th {
                background-color: #f2f2f2;
                text-align: left;
                padding: 8px;
            }

            /* Estilo para as células de dados */
            td {
                text-align: left;
                padding: 8px;
                border-bottom: 1px solid #ddd;
            }

            /* Estilo para células de dados alternadas (para listas zebradas) */
            tr:nth-child(even) {
                background-color: #f2f2f2;
            }

            /* Estilo para células de dados quando passa o mouse por cima */
            tr:hover {
                background-color: #ddd;
            }
          </style>
        </head>
        <body>
          <h1>Tabela de Medição</h1>
          <table>
            <thead>
              <tr>
                <th>ID</th>
                <th>Umidade</th>
                <th>Temperatura</th>
                <th>Ninho</th>
                <th>Timestamp</th>
              </tr>
            </thead>
            <tbody>
              ${rows.map((row) => `
                <tr>
                  <td>${row.id}</td>
                  <td>${row.umidade}</td>
                  <td>${row.temperatura}</td>
                  <td>${row.ninho}</td>
                  <td>${row.timestamp}</td>
                </tr>
              `).join('')}
            </tbody>
          </table>
        </body>
        <script>
          setTimeout(() => window.location.reload(), 1000)
        </script>
      </html>
    `;
    res.send(htmlTable);
  });
});


//setTimeout(() => window.location.reload(), 1000)
module.exports = router;