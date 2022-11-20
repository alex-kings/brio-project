const {spawn} = require("child_process")
const express = require('express')
const cors = require('cors')
const parser = require('body-parser')
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

/**
 * Get a selection of pieces from frontend. Generates the track by sending this selection to core. Return the track generated to frontend.
 */
app.post('/generate_track', async (req,res)=>{
    console.log("selection from user: ", req.body)

    // Get track from piece selection
    const coreMsg = await executeCore(req.body)
    console.log("cpp output: \n",coreMsg)

    let track
    // Get output track from file
    fs.readFile('../tracks/track_result.json', 'utf8', (err, data) => {
        if(err) {
            console.error(err)
            return
        }
        // return data as a javascript object

        track = JSON.parse(data)

        console.log(track)

        res.send(track)
    })
})

// Add piece to ressources
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

/**
 * Execute core algorithm.
 * A piece selection in raw Json format is passed. The function returns a track layout.
 */
async function executeCore(selection) {
    // Stringify piece selection
    const data = JSON.stringify(selection)
    
    const corePromise = new Promise((resolve, reject) => {
        // Pass input to cpp program
        const cprocess = spawn('../core/build/main', [data])
        
        try{
            cprocess.stdout.on('data',(data)=>{
                resolve(data.toString())
            })
        }
        catch(e){
            reject(e)
        }
    })
    // Log message printed on stdout by the core.
    const msg = await corePromise
    console.log("Core message: ", msg)
    return msg
}

app.listen(port, () => console.log(`API listening on port ${port}.`))
  