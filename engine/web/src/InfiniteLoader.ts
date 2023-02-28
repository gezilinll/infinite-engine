//@ts-ignore
import Module from "./wasm/InfiniteEngine";

export class InfiniteLoader {
    public static module: any = undefined;
    private static releaseMode: boolean = false;
    private static _wasmPath = "http://rqm1nmwwk.hn-bkt.clouddn.com/InfiniteCanvas.wasm";

    public static set wasmPath(path: string) {
        if (!InfiniteLoader.releaseMode) {
            this._wasmPath = path;
        }
    }

    public static async init() {
        return new Promise(async (resolve, reject) => {
            const instance = await Module({
                locateFile: (path: string) => {
                    return InfiniteLoader._wasmPath;
                },
            }).then((module: typeof Module) => {
                return module;
            }).catch((err: any) => {
                console.error(err);
                return undefined;
            });

            if (instance) {
                InfiniteLoader.module = instance;
                // @ts-ignore
                window.mainModule = instance;
            }
            if (InfiniteLoader.module) {
                resolve(this.module);
            } else {
                reject("!!!Load Wasm Error!!!");
            }
        });
    }
}