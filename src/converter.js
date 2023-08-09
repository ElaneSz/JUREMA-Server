const sqlite3 = require('sqlite3').verbose();
const createCsvWriter = require('csv-writer').createObjectCsvWriter;// Biblioteca para converter em arquivo .CSV

const dbPath = 'database.sqlite'; // Caminho para o banco de dados SQLite3
const outputPath = 'tabela.csv'; // Caminho para salvar o arquivo .CSV

const db = new sqlite3.Database(dbPath); // Conexão com o BD

const query = 'SELECT * FROM medicao'; // Consulta para selecionar os dados do banco de dados

db.all(query, (err, rows) => {
  if (err) {
    console.error('Erro ao executar a consulta:', err);
    db.close();
    return;
  }

  // Configura o cabeçalho do arquivo CSV
  const csvWriter = createCsvWriter({
    path: outputPath,
    header: [
      { id: 'umidade', title: 'Umidade' }, // Nome da coluna no banco de dados | Título da coluna no .CSV
      { id: 'temperatura', title: 'temperatura' },
      { id: 'ninho', title: 'Ninho' },
      { id: 'timestamp', title: 'TimesTamp' },
    ]
  });

  // Escreve os dados no arquivo CSV
  csvWriter.writeRecords(rows)
    .then(() => {
      console.log('Arquivo CSV criado com sucesso!');
      db.close();
    })
    .catch((err) => {
      console.error('Erro ao criar o arquivo CSV:', err);
      db.close();
    });
});
