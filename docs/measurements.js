/**
 * Make measurements
 */
const NUMBER_MEASUREMENTS = 10;

export function makeMeasurements(pieces) {
    createModule().then(({takeMeasurements}) => {
        // Stop loading
        document.getElementById("loadingPane").style.display = "none"
    
        // Generate!
        const res = takeMeasurements(pieces, NUMBER_MEASUREMENTS)
        console.log("RESULTS:")
        console.log(res)
    })
}
