const { exec } = require("child_process");

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

// Get all pieces from json file
app.get('/all_pieces', (req,res) => {
    let pieces = null
    try {
        pieces = JSON.parse(fs.readFileSync(JSON_FILE))
    } catch (error) {
        console.log("Couldn't fetch pieces!")
        console.log(error)
        return
    }
    res.send(pieces)
})

// Get piece selection from frontend
app.post('/send_piece_selection', (req,res)=>{
    savePieces(req.body)
    res.end()
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

// Send pieces to c++ program
function savePieces(selection){
    console.log(selection)
}

app.listen(port, () => console.log(`Hello world app listening on port ${port}!`))

// Execute command
function execute(command){
    exec(command, (error, stdout, stderr) => {
        if (error) {
            console.log(`error: ${error.message}`);
            return;
        }
        if (stderr) {
            console.log(`stderr: ${stderr}`);
            return;
        }
        console.log(`stdout: ${stdout}`);
    });
}
