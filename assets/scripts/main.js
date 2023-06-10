document.getElementById("responsive-button").onclick = toggleNavbar;
document.getElementById("close-nav").onclick = toggleNavbar;
document.querySelectorAll("header>nav>ul>li").forEach(item => {
    item.onclick = toggleNavbar;
})

function toggleNavbar() {
    const nav = document.querySelector("header>nav");
    if(!nav)
        return console.error("Navbar not found...");
    nav.classList.toggle("toggled");
}