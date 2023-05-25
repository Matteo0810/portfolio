document.getElementById("skills").onchange = (event) => {
    const skillId = event.target.value;
    for(const project of projectsNodes) {
        const hasSkillAssociated = project.getAttribute("data-skill-associated") === skillId;
        project.style.display = hasSkillAssociated || skillId === "all" ? "block" : "none";
    }
}