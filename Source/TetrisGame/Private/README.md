# Tetris Game Plugin Development Workflow

## Git Commit Guidelines

1. **Atomic Commits**: Each commit should represent a single logical change
2. **Descriptive Messages**: Use the format:
   ```
   type(scope): brief description

   Detailed explanation if needed
   ```
   Common types:
   - feat: New feature
   - fix: Bug fix
   - docs: Documentation changes
   - refactor: Code refactoring
   - test: Test additions/modifications

3. **Regular Pushes**: Push changes to GitHub after each verified change

## Standard Workflow

1. Make changes to the code
2. Stage changes:
   ```bash
   git add .
   ```
3. Commit with message:
   ```bash
   git commit -m "type(scope): description"
   ```
4. Push to GitHub:
   ```bash
   git push origin main
   ```

## Branching Strategy

- `main` branch: Stable production code
- Feature branches: `feature/description`
- Hotfix branches: `hotfix/description`

## Code Review

All changes should be:
1. Tested locally
2. Reviewed via pull requests
3. Approved before merging
