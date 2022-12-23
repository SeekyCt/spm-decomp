## Naming

- Use official names when possible (from TTYD symbol map, asserts, etc.)
- Name types in PascalCase (for example, `EvtWork`)
- Name functions, variables and fields in camelCase (for example, `evtEntry`)
- Name file-local symbols as `<filename>_<name>` with name in camelCase (for example, `evtmgr_someVariable`)
- Name function-local symbols as `<functionname>_<name>` with name in camelCase
- Name unknown functions present in all versions `func_80XXXXXX` and data `lbl_80XXXXXX` with their eu0 addresses
    - If not present in that version, name it `<type>_<version>_80XXXXXX` for a version it exists in

## Formatting

- Indent with 4 spaces
- Add a newline before a `{`
- Keep line length to 100 max
- Use `#pragma once` in every header
    - If the file has a top comment, the pragma comes after this
- Include `<common.h>` in every file
- Use angle brackets for all includes
- Order includes as:
    - Root level includes (`common.h`, `evt_cmd.h`, etc)
    - `spm/` includes
    - `wii/` includes
    - `nw4r/` includes
    - `msl/` includes
- Wrap files in CPP_WRAPPER from where includes finish to the end of the file
    - The name passed in should be `folder::filename` (for example, `spm::evtmgr`)
    - `common.h` and the variants of `evt_cmd.h` are exceptions to this
    - This won't apply to nw4r headers code they're done properly
- Wrap file-local and function-local symbols in DECOMP_STATIC in headers (for example, `DECOMP_STATIC(evtmgr_work)`)
- Add offset comments on the left of every struct member
- Add value comments on the left of every enum member
- In offset/value comments, use uppercase letters hex
- In code, use lowercase letters in hex
- Add a SIZE_ASSERT for every struct (for example, `SIZE_ASSERT(EvtWork, 0xa0)`)
    - For variable length structs, use an OFFSET_ASSERT of the final member (for example, `OFFSET_ASSERT(NPCAnim, tribeAnims, 0x48)`)
- Don't add address comments
- Include every function in a file, using UNKNOWN_FUNCTION if it's undocumented
    - Add comments indicating the file doesn't end there if the file boundaries are unknown

## Symbols

- Every symbol should be provided for eu0
    - Other regions are not currently required, but accepted
