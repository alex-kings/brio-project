const express = require('express')
const cors = require('cors')

const app = express()
const port = 3000

app.use(cors())

app.get('/', (req, res) => {
    res.send('Hello World, from express');
})

app.get('/api/hello',(req,res) => {
    res.send({'hello':'hello'})
})

app.listen(port, () => console.log(`Hello world app listening on port ${port}!`))

