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

module.exports = router;