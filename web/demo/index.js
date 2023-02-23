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

  // Set line width
  context2D.lineWidth = 10;
  // Wall
  context2D.strokeRect(75, 140, 150, 110);
  // Door
  context2D.fillRect(130, 190, 40, 60);
  // Roof
  context2D.beginPath();
  context2D.moveTo(50, 140);
  context2D.lineTo(150, 60);
  context2D.lineTo(250, 140);
  context2D.closePath();
  context2D.stroke();

  canvas.delete();
  console.log("ALL END");
});