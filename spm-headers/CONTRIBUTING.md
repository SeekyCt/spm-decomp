## Naming

- Use official names when possible (from TTYD symbol map, asserts, etc.), and try to follow the style otherwise
    - Use prefixes based on the area of code (ex. `marioMain`, `evtRestart`)
    - Follow patterns like `work`, `wp`, `entry`, `onoff`
- Name types in PascalCase (for example, `EvtWork`)
- Name functions, variables and fields in camelCase (for example, `evtEntry`)
- Name file-local symbols as `<filename>_<name>` with `name` in camelCase (for example, `evtmgr_someVariable`)
- Name function-local symbols as `<functionname>_<name>` with `name` in camelCase
- Name unknown functions present in all versions `func_80XXXXXX` and data `lbl_80XXXXXX` with their eu0 addresses
    - If not present in that version, name it `<type>_<version>_80XXXXXX` for a version it exists in
- Name evt scripts in snake case, with the suffix `_evt`
- Name user funcs in snake case, with the prefix `evt_`

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
    - This won't apply to nw4r headers' code they're done properly
- Wrap file-local and function-local symbols in DECOMP_STATIC in headers (for example, `DECOMP_STATIC(evtmgr_work)`)
- Add offset comments on the left of every struct member
- Add value comments on the left of every enum member
- In offset/value comments, use uppercase letters hex
- In code, use lowercase letters in hex
- Add a SIZE_ASSERT for every struct (for example, `SIZE_ASSERT(EvtWork, 0xa0)`)
    - For variable length structs, use an OFFSET_ASSERT of the final member (for example, `OFFSET_ASSERT(NPCAnim, tribeAnims, 0x48)`)
- Don't add address comments
- Where possible, include every function in a file, using UNKNOWN_FUNCTION if it's undocumented
    - Add comments indicating the file doesn't end there if the file boundaries are unknown (`// more`)
- Don't add semi-colons after defines that already include them (`SIZE_ASSERT`, `OFFSET_ASSERT`, `DECOMP_STATIC`, `USING`, `EVT_DECLARE_USER_FUNC`, `EVT_UNKNOWN_USER_FUNC`, `EVT_DECLARE`)
- Define types at the top of a file, then data, then functions
- Keep functions and data in address order
- Don't use :: to access namespaced types, add `USING` statements at the start of the `CPP_WRAPPER`

## Symbols

- Every symbol should be provided for eu0
    - Other regions are not currently required, but accepted
    - If you notice any symbols are missing, they might only be present in the [decomp symbols yml](https://github.com/SeekyCt/spm-decomp/blob/master/config/symbols.yml)
- The eu0 lst should be sorted in address order and split by source file
    - When symbols from multiple sections are added in a source file, symbols should be split by section
- Other lsts are sorted into the batches that they were ported in
- Only add symbols for the start of functions and data, not in the middle
    - Mods wanting to use these as patch addresses should use offsets from the actual symbols in their code
- Rel symbols should be in the format `moduleID,sectionId,offset` rather than raw addresses

## Utilities

- [This](https://cdn.discordapp.com/attachments/610974864706371585/1130952580684587239/symbols.h) is an auto-generated list of UNKNOWN_FUNCTION declarations for all unknown functions (naming might be out of date)
- [rel_offset.py](https://cdn.discordapp.com/attachments/610974864706371585/1151268789019750430/rel_offset.py) converts addresses contained by a rel in a RAM dump to the lst symbol format
