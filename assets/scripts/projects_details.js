const ANIMATION_TIME = 1e3;

function initProjectDetails() {
    for(const project of Array.from(projectsNodes)) {
        project.querySelector("button").onclick = () => {
            const projectId = project.getAttribute("data-project-id");
            openProjectDetails(projects[parseInt(projectId)]);
        }
    }
}

function createTmpAnimationElement(animationType) {
    const tmpElement = document.createElement("div");
    tmpElement.classList.add("tmp-animation-element", animationType);
    document.body.append(tmpElement);
    return tmpElement;
}

function openProjectDetails(projectData) {
    const tmpElement = createTmpAnimationElement("open");
    document.body.style.overflow = "hidden";
    setTimeout( () => {
        document.body.removeChild(tmpElement);
        document.body.insertAdjacentHTML("beforeend", getProjectHTMLString(projectData));

        document.querySelector("#project-details .close").onclick = closeProjectDetails;
        initProjectDetailsCarousel();
    }, ANIMATION_TIME);
}

function initProjectDetailsCarousel() {
    const carousel = document.querySelector("#project-details .carousel");
    const actualImg = carousel.querySelector("figure>img");
    const thumbnails = carousel.querySelectorAll("div>img");

    for(const thumbnail of Array.from(thumbnails)) {
        thumbnail.onclick = () => {
            actualImg.src = thumbnail.src;
            actualImg.alt = thumbnail.alt;
            carousel.querySelector("figure>figcaption").textContent = thumbnail.alt;
        }
    }
}

function closeProjectDetails() {
    document.getElementById("project-details").remove();
    document.body.style.overflow = "visible";
    const tmpElement = createTmpAnimationElement("close");
    setTimeout(() => document.body.removeChild(tmpElement), ANIMATION_TIME*1.5);
}

function getProjectHTMLString(project) {
    const currentScreen = project.screens[0];
    return `
        <div id="project-details">
            <button class="close">
                <span class="icon times"></span>
            </button>
        
            <div>
                <article>
                    <h1>${project.title}</h1>
                    <h2>${project.subtitle}</h2>
        
                    <h3>Étapes de réalisation</h3>
                    <ul>
                        ${project.steps.map((step, index) => 
                            `<li>
                                <span>Etape ${index+1}</span> ${step}
                            </li>`
                        ).join('')}
                    </ul>
        
                    <h3>Compétences développées</h3>
                    <ul>
                        ${project.skills.map(({ skill, label }) =>
                            `<li>
                                <span>${skill}</span> ${label}
                            </li>`
                        ).join('')}
                    </ul>
        
                    ${project.deliverables.length > 0 ? `
                        <h3>Livrables</h3>
                        <ul>
                            ${project.deliverables.map(deliverable => 
                              `<li>
                                   <a href="assets/data/${project.path}/deliverables/${deliverable}" download="${deliverable}">
                                       <button>
                                            ${deliverable} 
                                            <span class="icon download-reversed"></span>
                                        </button>
                                   </a>
                              </li>
                              `).join('')}
                        </ul>`
                    : ''}
                </article>
                <div class="carousel">
                    <figure>
                        <img src="assets/data/${project.path}/screens/${currentScreen.src}" alt="${currentScreen.legend}">
                        <figcaption>
                            ${currentScreen.legend}
                        </figcaption>
                    </figure>
                    <div>
                        ${project.screens.map(({ legend, src }) =>
                            `<img src="assets/data/${project.path}/screens/${src}" alt="${legend}">`
                        ).join('')}
                    </div>
                </div>
            </div>
        </div>
    `;
}