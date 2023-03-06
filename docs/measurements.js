/**
 * Make measurements
 */

export function makeMeasurements(pieces) {
    createModule().then(({takeMeasurements}) => {
        // Stop loading
        document.getElementById("loadingPane").style.display = "none"
    
        // Generate!
        const res = takeMeasurements(pieces, 100)
        console.log("RESULTS:")
        console.log(res)
    })
}
