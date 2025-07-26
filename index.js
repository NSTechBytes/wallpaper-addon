const addon = require('./build/Release/wallpaper-addon.node');

/**
 * Sets a window behind desktop icons (as wallpaper)
 * @param {string} windowHandle - The window handle as a string
 * @returns {boolean} - True if successful, false otherwise
 */
function setWindowBehindDesktop(windowHandle) {
    if (typeof windowHandle !== 'string') {
        throw new TypeError('Window handle must be a string');
    }
    return addon.setWindowBehindDesktop(windowHandle);
}

/**
 * Gets the WorkerW window handle
 * @returns {string|null} - The WorkerW window handle as string, or null if not found
 */
function getWorkerWindow() {
    return addon.getWorkerWindow();
}

/**
 * Restores a window from behind desktop (removes from wallpaper)
 * @param {string} windowHandle - The window handle as a string
 * @returns {boolean} - True if successful, false otherwise
 */
function restoreWindow(windowHandle) {
    if (typeof windowHandle !== 'string') {
        throw new TypeError('Window handle must be a string');
    }
    return addon.restoreWindow(windowHandle);
}

/**
 * Checks if a window handle is valid
 * @param {string} windowHandle - The window handle as a string
 * @returns {boolean} - True if valid, false otherwise
 */
function isValidWindow(windowHandle) {
    if (typeof windowHandle !== 'string') {
        return false;
    }
    return addon.isValidWindow(windowHandle);
}

module.exports = {
    setWindowBehindDesktop,
    getWorkerWindow,
    restoreWindow,
    isValidWindow
};