install thsi image for mario and run it 
 pengbai/docker-supermario 


 ## 🚧 TODO

### 🎮 Game Rendering
- [ ] Make `luanti` game render via Docker (try using SDL via X11 forwarding or containerized VM)
- [ ] Explore Virtual Machine or GUI-enabled containers to support SDL rendering

### 🧱 Architecture
- [ ] Refactor backend into MVC (Model-View-Controller) architecture
  - [ ] Separate game logic (Model)
  - [ ] Isolate rendering/display logic (View)
  - [ ] Implement input/network handling (Controller)

### 🌐 Networking
- [ ] Integrate WebRTC for multiplayer or live game stream
- [ ] Create signaling mechanism (if using WebRTC peer-to-peer)

### ☁️ Deployment
- [ ] Deploy on Azure/AWS (Free Tier)
  - [ ] Set up Docker container in Azure App Service or AWS EC2
  - [ ] Configure persistent storage for game state
  - [ ] Enable GUI rendering via noVNC or X11 forwarding (if needed)

### 📄 Documentation & Extras
- [ ] Document game setup and launch instructions
- [ ] Add development and deployment instructions
- [ ] Optional: Add GitHub Actions for CI/CD
