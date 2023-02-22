import {
  WasmLoader
} from "../src/WasmLoader";
import {
  Canvas
} from "../src/Canvas";


WasmLoader.init().then(async () => {
  let canvas = new Canvas("isurface");
  console.log(canvas);
  let context2D = canvas.getContext("2d");
  console.log(context2D);
  context2D.drawSomething();
});