const express = require('express')
const cors = require('cors')
const parser = require('body-parser')

//File writer
const fs = require('fs')
const JSON_FILE = '../ressources/pieces.json'

const app = express()
const port = 5000

app.use(cors())

// Configuring express to use body-parser as middle-ware
app.use(parser.urlencoded({ extended: false }));
app.use(parser.json());


// Get API calls for frontend
require('./frontendRouter')(app)


// Add a new piece to database
app.post('/add_piece', (req,res) => {
    const pieceData = req.body

    try{
        addPiece(pieceData)
    }
    catch(e){
        console.log("Couldn't add piece to json file.")
        console.log(e)
        return
    }

    res.json('Piece added to database.')
})

function addPiece(piece){
    console.log('adding piece...')

    // Get current file contents
    const data = JSON.parse(fs.readFileSync(JSON_FILE))
    // Add piece to current pieces
    data[piece.id] = piece
    // Save new pieces to file
    const newData = JSON.stringify(data)
    fs.writeFile(JSON_FILE, newData, err=>{
        if(err) console.log(err)
    })

}

app.listen(port, () => console.log(`Hello world app listening on port ${port}!`))

