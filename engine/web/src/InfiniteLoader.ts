//@ts-ignore
import Module from "./wasm/InfiniteEngine";

export class InfiniteLoader {
    public static module: any = undefined;
    public static wasmPath = "http://rqm1nmwwk.hn-bkt.clouddn.com/InfiniteEngine.wasm";

    public static async init() {
        return new Promise(async (resolve, reject) => {
            const instance = await Module({
                locateFile: (path: string) => {
                    return InfiniteLoader.wasmPath;
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