const wallpaperAddon = require('./index.js');

/**
 * Example: Basic usage of wallpaper-addon
 */

console.log('=== Wallpaper Addon Basic Usage Example ===\n');

// Step 1: Get WorkerW window handle
console.log('Step 1: Getting WorkerW window handle...');
const workerHandle = wallpaperAddon.getWorkerWindow();

if (!workerHandle) {
    console.error('❌ Failed to get WorkerW window handle');
    console.log('This might happen if:');
    console.log('- Windows desktop is not ready');
    console.log('- Running on unsupported Windows version');
    console.log('- Need administrator privileges');
    process.exit(1);
}

console.log(`✅ WorkerW handle obtained: ${workerHandle}`);
console.log(`✅ WorkerW is valid: ${wallpaperAddon.isValidWindow(workerHandle)}\n`);

// Step 2: Example with a hypothetical window handle
console.log('Step 2: Example window manipulation...');

// In a real scenario, you would get this from:
// - Electron: win.getNativeWindowHandle().toString()
// - Win32 API calls
// - External tools like Spy++
const exampleWindowHandle = "2230926"; // This would be a real handle

console.log(`Testing with example handle: ${exampleWindowHandle}`);
console.log(`Is example handle valid: ${wallpaperAddon.isValidWindow(exampleWindowHandle)}`);

// This will return false since it's not a real window handle
const setResult = wallpaperAddon.setWindowBehindDesktop(exampleWindowHandle);
console.log(`Attempt to set behind desktop: ${setResult}`);

// Step 3: Demonstrate error handling
console.log('\nStep 3: Error handling examples...');

try {
    // This should throw a TypeError
    wallpaperAddon.setWindowBehindDesktop(12345);
} catch (error) {
    console.log(`✅ Caught expected error: ${error.message}`);
}

try {
    // This should return false
    const invalidResult = wallpaperAddon.isValidWindow("not_a_number");
    console.log(`✅ Invalid handle check result: ${invalidResult}`);
} catch (error) {
    console.log(`Unexpected error: ${error.message}`);
}

console.log('\n=== Example Complete ===');
console.log('\nTo use with real windows:');
console.log('1. Get a real window handle from your application');
console.log('2. Call wallpaperAddon.setWindowBehindDesktop(handle)');
console.log('3. Call wallpaperAddon.restoreWindow(handle) to restore');

// Utility function example
function createWallpaperManager() {
    return {
        setAsWallpaper: (windowHandle) => {
            if (!wallpaperAddon.isValidWindow(windowHandle)) {
                throw new Error('Invalid window handle');
            }
            return wallpaperAddon.setWindowBehindDesktop(windowHandle);
        },
        
        restore: (windowHandle) => {
            if (!wallpaperAddon.isValidWindow(windowHandle)) {
                throw new Error('Invalid window handle');
            }
            return wallpaperAddon.restoreWindow(windowHandle);
        },
        
        isWorkerWindowAvailable: () => {
            const handle = wallpaperAddon.getWorkerWindow();
            return handle !== null;
        }
    };
}

console.log('\n=== Utility Manager Example ===');
const manager = createWallpaperManager();
console.log(`Worker window available: ${manager.isWorkerWindowAvailable()}`);