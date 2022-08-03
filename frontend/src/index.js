// Send fetch request to backend
const response = await fetch('http://localhost:3000/api/hello')
const result = await response.json()
console.log(result)