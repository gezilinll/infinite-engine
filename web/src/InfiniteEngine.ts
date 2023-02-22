//@ts-ignore
import Module from "./wasm/InfiniteEngine";

export class InfiniteEngine {
    public static rawModule: any = undefined;

    public static async init() {
        return new Promise(async (resolve, reject) => {
            const instance = await Module({
                locateFile: (path: string) => {
                    return path;
                },
            }).then((module: Module) => {
                return module;
            }).catch((err: any) => {
                console.log("BBBB");
                console.error(err);
                return undefined;
            });

            console.log("CCCC");
            if (instance) {
                InfiniteEngine.rawModule = instance;
                // @ts-ignore
                window.mainModule = instance;
            }
            if (InfiniteEngine.rawModule) {
                resolve(this.rawModule);
            } else {
                reject("!!!Load Wasm Error!!!");
            }
        });
    }
}