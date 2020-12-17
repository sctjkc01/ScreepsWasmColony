const wasm_loader = require("wasm_loader");
const m = wasm_loader("loop_mod", "loop");

module.exports.loop = function() {
    console.log("=======" + Game.time);
    if(typeof m.loop === "undefined") return;

    m.loop(Game, Memory, RawMemory);
};