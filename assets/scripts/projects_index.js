const projects = [];
let projectsNodes;

async function loadProjects() {
    const list = await getFileContent("assets/data/projects.json");
    for(const el of list)
        projects.push(el);
}

loadProjects()
    .then(() => {
        loadProjectList();
        projectsNodes = document.querySelectorAll("#projects>section>article");
        initProjectDetails();
    })
    .catch(console.error);

function loadProjectList() {
    const projectListNode = document.getElementById("project-list");
    for(let i = 0; i < projects.length;i++) {
        const project = projects[i];
        projectListNode.insertAdjacentHTML("beforeend", `
            <article data-skill-associated="${project.options.associated_with}" data-project-id="${i}">
                <h1>${project.title}</h1>
                <h4>${project.subtitle}</h4>
    
                <button>
                    Voir le projet
                    <span class="icon arrow-right"></span>
                </button>
            </article>
        `);
    }
}