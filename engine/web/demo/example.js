import { InfiniteLoader } from "../src/InfiniteLoader";
import { InfiniteEngine } from "../src/InfiniteEngine";

const { default: wasmURL } = await import('./InfiniteEngine.wasm?url');
InfiniteLoader.wasmPath = wasmURL;

InfiniteLoader.init().then(async () => {
    let engine = new InfiniteEngine("canvas");
});