const express = require('express')
const cors = require('cors')
const parser = require('body-parser')

const app = express()
const port = 3000

app.use(cors())

// Configuring express to use body-parser as middle-ware
app.use(parser.urlencoded({ extended: false }));
app.use(parser.json());


// Add a new piece to database
app.post('/add_piece', (req,res) => {
    const pieceData = req.body

    console.log(pieceData)

    res.json('Piece added to database.')
})

app.listen(port, () => console.log(`Hello world app listening on port ${port}!`))

