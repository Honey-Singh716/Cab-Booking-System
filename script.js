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


    async function fetchMainRoadPolylines(center, radiusMeters){
        const [lat, lon] = center;
        const query = `[
            out:json
        ];
        (
          way["highway"~"^(motorway|trunk|primary|secondary|tertiary)$"](around:${Math.round(radiusMeters)},${lat},${lon});
        );
        out geom;`;
        const res = await fetch(OVERPASS, { method: 'POST', body: query, headers: { 'Content-Type': 'text/plain' } });
        if (!res.ok) throw new Error('Overpass request failed');
        const data = await res.json();
        // Returning array of polylines, each polyline is [[latitude,longitude], ...]
        return (data.elements || []).filter(e => e.type === 'way' && Array.isArray(e.geometry)).map(e => e.geometry.map(p => [p.lat, p.lon]));
    }

    function samplePointsFromPolylines(polylines, count){
        const points = [];
        if (!polylines.length) return points;
        for (let i = 0; i < count; i++) {
            const way = polylines[Math.floor(Math.random() * polylines.length)];
            if (way.length < 2) { i--; continue; }
            const idx = Math.floor(Math.random() * (way.length - 1));
            const a = way[idx];
            const b = way[idx + 1];
            const t = Math.random();
            const lat = a[0] + (b[0] - a[0]) * t;
            const lng = a[1] + (b[1] - a[1]) * t;
            points.push([lat, lng]);
        }
        return points;
    }

    async function spawnNearbyCabs(center, count, radiusMeters){
        try {
            const polylines = await fetchMainRoadPolylines(center, radiusMeters);
            const pts = samplePointsFromPolylines(polylines, count);
            if (pts.length) {
                cabCoords = pts;
            } else {
                throw new Error('No main roads found');
            }
        } catch (e) {
            console.warn('Falling back to random spawn:', e.message || e);
            // Fallback: random around
            const lat = center[0];
            const lng = center[1];
            const metersPerDegLat = 111320;
            const metersPerDegLng = Math.cos(lat * Math.PI/180) * 111320;
            cabCoords = [];
            for (let i=0;i<count;i++){
                const r = Math.random() * radiusMeters;
                const theta = Math.random() * Math.PI * 2;
                const dx = (r * Math.cos(theta)) / metersPerDegLng;
                const dy = (r * Math.sin(theta)) / metersPerDegLat;
                cabCoords.push([lat + dy, lng + dx]);
            }
        }
        cabMarkers.forEach(m => map.removeLayer(m));
        cabMarkers = cabCoords.map(c => L.marker(c, { icon: iconHtml('marker-cab') }).addTo(map));
    }

})();