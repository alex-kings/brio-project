const express = require('express')
const cors = require('cors')

const app = express()
const port = 3000

app.use(cors())

app.post('/book', (req, res) => {
    const book = req.body;

    // Output the book to the console for debugging
    console.log(book);
    books.push(book);

    res.send('Book is added to the database');
});

// Add a new piece to database
app.post('/add_piece', (req,res) => {
    const pieceData = req.body

    console.log(pieceData)

    res.json('Piece added to database.')
})

app.listen(port, () => console.log(`Hello world app listening on port ${port}!`))

