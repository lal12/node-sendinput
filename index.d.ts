export enum KBDInputType{
    VirtualKey = 0, ScanCode = 1, Unicode = 2
};
export interface KBDInput{
    up?: boolean;
    val: number;
    type: KBDInputType;
}

export function SendInput(inputs: Array<KBDInput>|KBDInput);