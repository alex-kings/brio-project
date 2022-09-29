/**
 * Provides API functions for the frontend.
 */

const JSON_FILE = '../ressources/pieces.json'
const fs = require('fs')

module.exports = (app) => {

    // Get all pieces from json file
    app.get('/all-pieces', (req,res) => {
        const pieces = null
        try {
            pieces = JSON.parse(fs.readFile(JSON_FILE))
        } catch (error) {
            console.log("Couldn't fetch pieces!")
            console.log(error)
            return
        }
        res.send(pieces)
    })

}