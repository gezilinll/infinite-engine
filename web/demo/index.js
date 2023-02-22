console.log("111");

import {
  InfiniteEngine
} from "../src/InfiniteEngine";

console.log("111");

InfiniteEngine.init().then(async () => {
  InfiniteEngine.rawModule.SkiaTester.initLoader();
});