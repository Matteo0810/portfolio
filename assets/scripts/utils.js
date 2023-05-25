async function getFileContent(url) {
    try {
        const response = await fetch(url);
        return await response.json();
    } catch(error) {
        throw error;
    }
}