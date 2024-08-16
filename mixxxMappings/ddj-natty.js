// eslint-disable-next-line no-var
var MyController = {};

MyController.alpha = 1.0/8;
MyController.beta = MyController.alpha/32;

// The button that enables/disables scratching
// MyController.wheelTouch = function (channel, control, value, status, group) {
//     var deckNumber = script.deckFromGroup(group);
//     if ((status & 0xF0) === 0x90) {    // If button down
//   //if (value === 0x7F) {  // Some wheels send 0x90 on press and release, so you need to check the value
//         var alpha = 1.0/8;
//         var beta = alpha/32;
//         engine.scratchEnable(deckNumber, 128, 33+1/3, alpha, beta);
//     } else {    // If button up
//         engine.scratchDisable(deckNumber);
//     }
// }

// The wheel that actually controls the scratching
engine.scratchEnable(0, 20, 33+1/3, this.alpha, this.beta);
engine.scratchEnable(1, 20, 33+1/3, this.alpha, this.beta);
engine.scratchEnable(2, 20, 33+1/3, this.alpha, this.beta);
MyController.wheelTurn = function (channel, control, value, status, group) {
    
    // --- Choose only one of the following!
    
    
    const deckNum = channel + 1;

    //engine.scratchEnable(deckNum, 20, 33+1/3, this.alpha, this.beta);
    // B: For a control that centers on 0x40 (64):
    var newValue = value - 64;
    
    // --- End choice
    
    // In either case, register the movement
    
    //var deckNumber = script.deckFromGroup(group);
    if (engine.isScratching(deckNum)) {
        engine.scratchTick(deckNum, newValue); // Scratch!
    } else {
        //engine.setValue(group, 'jog', newValue * 0.8); // Pitch bend
    }
   // engine.scratchDisable(deckNum);
}