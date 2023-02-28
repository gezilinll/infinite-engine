//@ts-ignore
import Module from "./wasm/InfiniteCanvas";

export interface EmbindObject<T extends EmbindObject<T>> {
    clone(): T;
    delete(): void;
    deleteLater(): void;
    isAliasOf(other: any): boolean;
    isDeleted(): boolean;
}

export class CanvasLoader {
    public static module: any = undefined;
    private static releaseMode: boolean = true;
    private static _wasmPath = "http://rqm1nmwwk.hn-bkt.clouddn.com/InfiniteCanvas.wasm";

    public static set wasmPath(path: string) {
        if (!CanvasLoader.releaseMode) {
            this._wasmPath = path;
        }
    }

    public static async init() {
        console.log(this.module);
        return new Promise(async (resolve, reject) => {
            const instance = await Module({
                locateFile: (path: string) => {
                    return CanvasLoader._wasmPath;
                },
            }).then((module: typeof Module) => {
                return module;
            }).catch((err: any) => {
                console.error(err);
                return undefined;
            });

            if (instance) {
                CanvasLoader.module = instance;
                // @ts-ignore
                window.mainModule = instance;
            }
            if (CanvasLoader.module) {
                resolve(this.module);
            } else {
                reject("!!!Load Wasm Error!!!");
            }
        });
    }
}