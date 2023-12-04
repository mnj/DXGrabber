<script lang="ts">
    import { onMount, onDestroy } from 'svelte';
    import type { Event } from './Events'; // Importing the Event type
  
    let events: Event[] = [];
  
    async function fetchEvents() {
      try {
        const response = await fetch('/api/events');
        if (!response.ok) {
          throw new Error(`Error: ${response.statusText}`);
        }
        const data: Event[] = await response.json();
        events = data.slice(0, 10);
      } catch (error) {
        console.error('Failed to fetch events:', error);
      }
    }
  
    let interval: number;
  
    onMount(() => {
      fetchEvents(); // Initial fetch
      interval = setInterval(fetchEvents, 500); // Fetch every 0.5 seconds
  
      return () => {
        clearInterval(interval); // Clear interval when component is destroyed
      };
    });

    function formatDate(timestamp: number): string {
        const date = new Date(timestamp);
        const hours = String(date.getHours()).padStart(2, '0');
        const minutes = String(date.getMinutes()).padStart(2, '0');
        const seconds = String(date.getSeconds()).padStart(2, '0');
        const milliseconds = String(date.getMilliseconds()).padStart(3, '0');

        return `${hours}:${minutes}:${seconds}.${milliseconds}`;
    }
  </script>
  
  <main>
    <h1>Events</h1>
    <ul>
      {#each events as event}
        <li>
          <strong>{formatDate(event.timestamp)}</strong> - {event.description}
        </li>
      {/each}
  </main>