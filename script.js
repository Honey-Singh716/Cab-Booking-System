(function(){
    const OSRM = 'https://router.project-osrm.org';
    const profile = 'driving';
    const NOMINATIM = 'https://nominatim.openstreetmap.org/search';
    const OVERPASS = 'https://overpass-api.de/api/interpreter';

    function setInfo(msg){
        document.getElementById('lfInfo').textContent = msg;
    }

    function iconHtml(cls){
        return L.divIcon({ className: '', html: `<div class="${cls}"></div>`, iconSize: [14,14], iconAnchor: [7,7] });
    }

    const map = L.map('map').setView([28.6139, 77.2090], 12); //  by default New Delhi
    L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
        maxZoom: 19,
        attribution: '&copy; OpenStreetMap contributors'
    }).addTo(map);

    let pickupMarker = null;
    let dropMarker = null;
    let cabMarkers = [];
    let cabCoords = []; // [ [latitude, longitude], ... ]
    // Two distinct route layers: cab->pickup and pickup->drop
    let routeCabPickupLayer = L.geoJSON(null, { style: { color: '#f59e0b', weight: 5, opacity: 0.95 } }).addTo(map); // amber
    let routePickupDropLayer = L.geoJSON(null, { style: { color: '#22c55e', weight: 5, opacity: 0.95 } }).addTo(map); // green
    let animMarker = null;
    let animCoords = [];
    let animIdx = 0;
    let animHandle = null;

    function resetAll(keepPickup = false){
        const currentPickup = keepPickup ? pickupMarker : null;
        
        if (dropMarker) { map.removeLayer(dropMarker); dropMarker = null; }
        routeCabPickupLayer.clearLayers();
        routePickupDropLayer.clearLayers();
        cabMarkers.forEach(m => map.removeLayer(m));
        cabMarkers = [];
        cabCoords = [];
        stopAnim();
        
        if (!keepPickup && pickupMarker) {
            map.removeLayer(pickupMarker);
            pickupMarker = null;
            setInfo('Click on map to set pickup.');
            setTrip({ status: 'Idle', phase: '–', distance: '–', duration: '–', now: nowString(), eta: '–' });
        } else if (pickupMarker) {
            setInfo('Click on map to set drop location (optional).');
        }
    }

    function stopAnim(){
        if (animHandle) cancelAnimationFrame(animHandle);
        animHandle = null;
        if (animMarker) { map.removeLayer(animMarker); animMarker = null; }
        animCoords = [];
        animIdx = 0;
    }

    async function osrmTable(coords){
        // coordinates: [[latitude,longitude], ...]
        const locs = coords.map(c => `${c[1]},${c[0]}`).join(';');
        const url = `${OSRM}/table/v1/${profile}/${locs}?annotations=distance`;
        const res = await fetch(url);
        if (!res.ok) throw new Error('OSRM table failed');
        return res.json();
    }

    async function osrmRoute(a, b){
        const locs = `${a[1]},${a[0]};${b[1]},${b[0]}`;
        const url = `${OSRM}/route/v1/${profile}/${locs}?overview=full&geometries=geojson&alternatives=false&steps=false&annotations=false`;
        const res = await fetch(url);
        if (!res.ok) throw new Error('OSRM route failed');
        return res.json();
    }

})();