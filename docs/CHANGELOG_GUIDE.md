# Changelog change-trace standard

Every player-visible or troubleshooting-relevant change should identify where it was implemented.

## Required format

For each changelog item, include a **Change trace** block with:

- **File:** repository-relative path.
- **Lines:** exact line range from the commit that introduced the change.
- **Anchor:** nearest stable function, symbol, script label, trainer constant, map event, table entry, or other searchable identifier.
- **Commit:** short Git commit SHA when known.
- **Notes:** optional explanation of what to inspect if the feature misbehaves.

Example:

```md
- Changed Brock's team to Geodude / Nosepass / Onix / Lileep.
  - Change trace:
    - File: `src/data/trainers.party`
    - Lines: 410-438
    - Anchor: `TRAINER_BROCK`
    - Commit: `abc1234`
    - Notes: Trainer party composition and held items.
```

## Why both lines and anchors are recorded

Line numbers are useful for immediately opening the exact location, but they can drift when files gain or lose lines. The **anchor is the durable reference**. If the listed lines no longer match after later development, search the same file for the anchor.

## Multi-file changes

List every file that materially participates in the feature. For an event this might include:

- the map script;
- trainer data;
- text;
- flags/constants;
- a C handler.

Do not list generated files or incidental formatting changes.

## Pull-request discipline

Before merging a feature:

1. update `CHANGELOG.md`;
2. record the final file/line locations from the branch head;
3. include stable anchors;
4. record the commit or PR reference;
5. make sure gameplay testing notes are included where relevant.

This is intentionally more detailed than a normal public-facing changelog. RomHack's changelog is also a troubleshooting and learning document for Mike and Chris.
